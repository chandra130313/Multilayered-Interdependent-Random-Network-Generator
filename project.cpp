#include<iostream>
#include<map>
#include<cstdlib>
#include<ctime>
#include<vector>
#include<limits.h>
#include<stack>
#include<set>
#include<fstream>
using namespace std;
multimap< int,multimap<int,int> > intraedge;
multimap< int,multimap<int,multimap<int,int> > > interedge;
multimap<int,vector<int> > indegree;
multimap<int,vector<int> > outdegree;
vector<int> layerdegree;
multimap<int,float> probability_layer;
multimap<int,multimap<int,float> > probability_node_indegree;
multimap<int,multimap<int,float> > probability_node_outdegree;
multimap<int,vector<int> >degree;
multimap<int,multimap<int,int> > layerconnected;
int size;
int count;
int n;
stack<int> s;
//n=no of layer and size= no of nodes in network
bool visited[1000];
bool visited_reverse[1000];
void DFS(int index,multimap<int,int> edges)
{
   if(visited[index]!=true){
      visited[index]=true;
      }
   for(multimap<int,int>::iterator it=edges.begin();it!=edges.end();it++)
    {
      if((*it).first==index&&visited[(*it).second]!=true)
        DFS((*it).second,edges);

    }
   s.push(index);
}
void DFS_reverse(int index,multimap<int,int> reverse_edges)
{
   if(visited_reverse[index]!=true){
      visited_reverse[index]=true;
        count++;
      }
   for(multimap<int,int>::iterator it=reverse_edges.begin();it!=reverse_edges.end();it++)
    {
      if((*it).first==index&&visited_reverse[(*it).second]!=true)
        DFS_reverse((*it).second,reverse_edges);

    }
}
void find_connected(multimap<int,int> edges,int ss,float f,int layer)
{
   fstream ff;
   ff.open("AfterRandomAttack.txt",ios::out|ios::app);
   multimap<int,int>::iterator it=edges.begin();
   DFS((*it).first,edges);
   for(multimap<int,int>::iterator it=edges.begin();it!=edges.end();it++)
   {
      if(visited[(*it).first]!=true)
        DFS((*it).first,edges);
      else if(visited[(*it).second]!=true)
        DFS((*it).second,edges);
   }
   //reversing the edges
   multimap<int,int> reverse_edges;
   for(it=edges.begin();it!=edges.end();it++)
   {
       reverse_edges.insert(pair<int,int>((*it).second,(*it).first));

   }
   int max=0;
   while(!s.empty())
    {
      if(visited_reverse[s.top()]!=true){
        count=0;
        DFS_reverse(s.top(),reverse_edges);
        cout<<endl;
        }
      if(count>max)
        max=count;
        s.pop();
    }
   cout<<"The maximum size of connected componets is "<<max<<endl;
   cout<<"Size of Network is "<<ss<<endl;
   if(ss!=0)
   cout<<"The fraction of node connected is "<<float(max)/float(ss)<<endl;
   else
    cout<<"The fraction of node connected is "<<"0"<<endl;

   if(ss!=0)
   ff<<layer<<" "<<f<<" "<<float(max)/float(ss)<<endl;
   else
   ff<<layer<<" "<<f<<" "<<"0"<<endl;

   ff.close();

}
void calculate_max_connected_component(float f)
{
  //for each layer caculating maximum size of connected components
  int max=0;
  for(multimap<int,multimap<int,int> >::iterator it=intraedge.begin();it!=intraedge.end();it++)
   {
      while(!s.empty())
       s.pop();
      for(int i=0;i<100;i++){
        visited[i]=false;
        visited_reverse[i]=false;
        }
      count=0;
      multimap<int,int> m=(*it).second;
      cout<<"Layer is "<<(*it).first<<endl;
      bool countt[1000]={false};
      for(multimap<int,int>::iterator it1=m.begin();it1!=m.end();it1++)
       {
         if(countt[(*it1).first]==false)
            countt[(*it1).first]=true;
         if(countt[(*it1).second]==false)
            countt[(*it1).second]=true;

       }
      int ss=0;
      for(int i=0;i<1000;i++)
        {
          if(countt[i]==true)
            ss++;
        }
      find_connected(m,ss,f,(*it).first);


   }

}
void calculate_degree()
{
  for(multimap<int,multimap<int,int> >::iterator it=intraedge.begin(); it!=intraedge.end();it++)
   {
      vector<int> v;
      cout<<"Layer "<<(*it).first<<endl;
      for(int i=0;i<size;i++){
      multimap<int,int> m = (*it).second;
      int count=0;
      for(multimap<int,int>::iterator it1=m.begin();it1!=m.end();it1++)
        {
           if((*it1).first==i)
            count++;
        }
       cout<<"indegree of node "<<i<<"is "<<count<<endl;
       v.push_back(count);
     }
   indegree.insert(pair<int,vector<int> >((*it).first,v));
   }
    //calculating outdegree of network
  for(multimap<int,multimap<int,multimap<int,int> > >::iterator it=interedge.begin();it!=interedge.end();it++)
    {
       vector<int> v;
       int layercount=0;
       cout<<"Layer "<<(*it).first<<endl;
       for(int i=0;i<size;i++)
         {
            multimap<int,multimap<int,int> > m;
            m=(*it).second;
            int count=0;
            for(multimap<int,multimap<int,int> >::iterator it1=m.begin();it1!=m.end();it1++)
              {
                 multimap<int,int> m1;
                 m1=(*it1).second;
                 multimap<int,int>::iterator it2=m1.begin();
                 if((*it2).first==i)
                   count++;
              }
            layercount+=count;
            cout<<"The outdegree of node "<<i<<" is "<<count<<endl;
            v.push_back(count);
         }
        layerdegree.push_back(layercount);
        outdegree.insert(pair<int,vector<int> >((*it).first,v));
    }
  //The degree of connection of each layer with other layer
  for(multimap<int,multimap<int,multimap<int,int> > >::iterator it=interedge.begin();it!=interedge.end();it++)
   {
      int layer1=(*it).first;
      multimap<int,int> m1;
        cout<<"Layer ------------- "<<layer1<<endl;
            for(int j=0;j<n;j++)
               {
                    multimap<int,multimap<int,int> >::iterator it1;
                    multimap<int,multimap<int,int> > m;
                    m=(*it).second;
                    int count=0;
                    for(it1=m.begin();it1!=m.end();it1++)
                      {
                         if((*it1).first==j)
                           count++;
                      }
                   cout<<"Layer "<<j<<" degree "<<count<<endl;
                   m1.insert(pair<int,int>(j,count));
               }
        layerconnected.insert(pair<int,multimap<int,int> >(layer1,m1));
   }

 //calculating probability of each layer to be involved in making edges
 int sum1=0;
 for(int i=0;i<layerdegree.size();i++)
  {
     cout<<"Layer degree of "<<i<<" is "<<layerdegree[i]<<endl;
     sum1+=layerdegree[i];
  }
  for(int i=0;i<n;i++)
   {
      float p=float(layerdegree[i])/float(sum1);
      cout<<"Probability of layer "<<i<<" is "<<p<<endl;
      probability_layer.insert(pair<int,float>(i,p));
   }
  //calculating probability of each node in each layer to be involved in making edges
  multimap<int,int> sum;
  for(int i=0;i<n;i++)
   {
      multimap<int,vector<int> >::iterator it;
      it=indegree.find(i);
      vector<int> v;
      v=(*it).second;
      int total=0;
       for(int j=0;j<v.size();j++)
        {
          cout<<"indegree of node "<<j<<" is "<<v[j]<<endl;
          total+=v[j];
        }
       cout<<"Total of layer "<<i<<" is "<<total<<endl;
       sum.insert(pair<int,int>(i,total));

   }
  for(int i=0;i<n;i++)
   {
      multimap<int,vector<int> >::iterator it;
      it=indegree.find(i);
      vector<int> v;
      v=(*it).second;
      multimap<int,int>::iterator it1;
      multimap<int,float>  m;
      it1=sum.find(i);
      int t=(*it1).second;
       for(int j=0;j<v.size();j++)
        {
          float p=float(v[j])/float(t);
          m.insert(pair<int,float>(j,p));
          cout<<"Probability of node "<<j<<" of layer "<<i<<" is "<<p<<endl;
        }
       probability_node_indegree.insert(pair<int,multimap<int,float> >(i,m));
     }
   //calculating probability of each node in each in degree to be involved in each layer based on outdegree

  multimap<int,int> sum2;
  for(int i=0;i<n;i++)
   {
      multimap<int,vector<int> >::iterator it;
      it=outdegree.find(i);
      vector<int> v;
      v=(*it).second;
      int total=0;
       for(int j=0;j<v.size();j++)
        {
          cout<<"outdegree of node "<<j<<" is "<<v[j]<<endl;
          total+=v[j];
        }
       cout<<"Total of layer "<<i<<" is "<<total<<endl;
       sum2.insert(pair<int,int>(i,total));

   }
  for(int i=0;i<n;i++)
   {
      multimap<int,vector<int> >::iterator it;
      it=outdegree.find(i);
      vector<int> v;
      v=(*it).second;
      multimap<int,int>::iterator it1;
      multimap<int,float>  m;
      it1=sum2.find(i);
      int t=(*it1).second;
       for(int j=0;j<v.size();j++)
        {
          float p;
          if(t!=0){
           p=float(v[j])/float(t);
          }
          else
            p=0;
          m.insert(pair<int,float>(j,p));
          cout<<"Probability of node "<<j<<" of layer "<<i<<" is "<<p<<endl;
        }
       probability_node_outdegree.insert(pair<int,multimap<int,float> >(i,m));
     }
 }
void AddNewNode()
{
  float p;
  int sizeoffirstlayer,sizeofsecondlayer;
  int r=rand()%100;
  p=float(r)/float(100);
  vector<int> v;//layer having degree more than the threshold
  for(int i=0;i<layerdegree.size();i++)
  {
     multimap<int,float>::iterator it;
     it=probability_layer.find(i);
     if((*it).second>p){
      cout<<"Layer "<<i<<" probability is "<<(*it).second<<" with thrashold "<<p<<endl;
      v.push_back(i);
      }
  }
  //Choosing prefferred layer
     int index;
     index=rand()%n;
  cout<<"Chosen Layer is  "<<index<<" Thrashold "<<p<<endl;
  //Choosing preferred node in prefered layer
  int r1=rand()%100;
  float p1=float(r1)/float(100);
  multimap<int,multimap<int,float> >::iterator it;
  it=probability_node_indegree.find(index);
  multimap<int,float> m;
  vector<int> v1;
  for(multimap<int,float>::iterator it1=m.begin();it1!=m.end();it1++)
   {

      cout<<"Thrashold "<<p1<<" Node "<<(*it1).first<<" probability "<<(*it1).second<<endl;
      if((*it1).second>p1){
       v1.push_back((*it1).first);
       }
   }
   int index2;
   if(v1.size()!=0){
   index2=rand()%v1.size();
   index2=v1[index2];
   }
   else
    {
      multimap<int,vector<int> >::iterator it_check;
      it_check=indegree.find(index);
      vector<int> v_check;
      v_check=(*it_check).second;
      sizeoffirstlayer=v_check.size();
      index2=rand()%v_check.size();
    }
   cout<<"Choosen Node in Layer "<<index<<" is "<<index2<<endl;
   int index3;                  //index of second layer to make interedge
   if(v.size()!=0){
   index3=rand()%v.size();
   index3=v[index3];
   if(index==index3)
    index3=(index3+1)%n;
     }
    else
      {
        index3=rand()%n;
        if(index==index3)
          index3=(index3+1)%n;
      }
   //choosing node in preferred layer in second prefered layer
  cout<<"Choosen Second Layer "<<index3<<" Thrashold "<<p<<endl;
  int r2=rand()%100;
  float p2=float(r2)/float(100);
  it=probability_node_outdegree.find(index3);
  multimap<int,float> m_1;
  vector<int> v2;
  for(multimap<int,float>::iterator it1=m_1.begin();it1!=m_1.end();it1++)
   {
      if((*it1).second>p2)
       v2.push_back((*it1).first);
   }
   int index4;
   if(v2.size()!=0){
     index4=rand()%v2.size();
     index4=v2[index4];
   }
   else{

      multimap<int,vector<int> >::iterator it_check;
      it_check=outdegree.find(index3);
      vector<int> v_check;
      v_check=(*it_check).second;
      sizeofsecondlayer=v_check.size();
      index4=rand()%v_check.size();
      cout<<"Index of second node "<<index4<<" vector size "<<v_check.size()<<endl;                   //index of node in second prefered layer to make interedges

    }
  //making new intraedge
  cout<<"Choose Node in Layer "<<index3<<"  is "<<index4<<" Thrashold "<<p2<<endl;
 multimap<int,int> m1;
 multimap<int,multimap<int,int> >::iterator it1;
 it1=intraedge.find(index);
 m1=(*it1).second;
 m1.insert(pair<int,int>(sizeoffirstlayer,index2));
 m1.insert(pair<int,int>(index2,sizeoffirstlayer));
 (*it1).second=m1;
 //making new interedge
 multimap<int,multimap<int,multimap<int,int> > >::iterator it2;
 it2=interedge.find(index);
 multimap<int,multimap<int,int> > m2;
 m2=(*it2).second;
 multimap<int,int> m3;
 m3.insert(pair<int,int>(sizeoffirstlayer,index4));
 m3.insert(pair<int,int>(index4,sizeoffirstlayer));
 m2.insert(pair<int,multimap<int,int> >(index3,m3));
 (*it2).second=m2;
 //Updation of indegree
 multimap<int,vector<int> >::iterator it_indegree;
 it_indegree=indegree.find(index);
 vector<int> v_indegree;
 v_indegree=(*it_indegree).second;
  v_indegree.push_back(1);
 int check=v_indegree[index2]+1;
 v_indegree[index2]=check;
 cout<<v_indegree[index2]<<" index is "<<index2<<" vector size is "<<v_indegree.size()<<" size is "<<size<<endl;
 cout<<"Updated value :"<<v_indegree[index2]<<endl;
 (*it_indegree).second=v_indegree;
 //updation of outdegree
 multimap<int,vector<int> >::iterator it_outdegree;
 it_outdegree=outdegree.find(index3);
 vector<int> v_outdegree;
 v_outdegree=(*it_outdegree).second;
 cout<<"Index of second layer node "<<index4<<"size of vector "<<v_outdegree.size()<<endl;
  check=v_outdegree[index4]+1;
  v_outdegree[index4]=check;
 (*it_outdegree).second=v_outdegree;
 it_outdegree=outdegree.find(index);
 v_outdegree=(*it_outdegree).second;
 v_outdegree.push_back(1);
 (*it_outdegree).second=v_outdegree;
 //updation of probability of indegree
 multimap<int,int> sum;
  for(int i=0;i<n;i++)
   {
      multimap<int,vector<int> >::iterator it;
      it=indegree.find(i);
      vector<int> v;
      v=(*it).second;
      int total=0;
       for(int j=0;j<v.size();j++)
        {
          total+=v[j];
        }
       cout<<"Layer "<<i<<" Total "<<total<<endl;
       sum.insert(pair<int,int>(i,total));

   }
  multimap<int,int>::iterator it_sum;
  it_sum=sum.find(index);
  int sum_total_indegree=(*it_sum).second;
  float p_indegree=float(v_indegree[index2])/float(sum_total_indegree);
  cout<<"Probability "<<p_indegree<<endl;
  multimap<int,multimap<int,float> >::iterator it_prob_indegree;
  it_prob_indegree=probability_node_indegree.find(index);
  multimap<int,float> m_indegree;
  m_indegree=(*it_prob_indegree).second;
  multimap<int,float>::iterator it_prob;
  it_prob=m_indegree.find(index2);
  (*it_prob).second=p;
  (*it_prob_indegree).second=m_indegree;
//updation of probability based on outdegree
  multimap<int,int> sum1;
 for(int i=0;i<n;i++)
   {
      multimap<int,vector<int> >::iterator it;
      it=outdegree.find(i);
      vector<int> v;
      v=(*it).second;
      int total=0;
       for(int j=0;j<v.size();j++)
        {
          total+=v[j];
        }
       sum1.insert(pair<int,int>(i,total));

   }
  multimap<int,int>::iterator it_sum1;
  it_sum1=sum1.find(index3);
  int sum_total_outdegree=(*it_sum1).second;
  float p_outdegree=float(v_outdegree[index4])/float(sum_total_outdegree);
  multimap<int,multimap<int,float> >::iterator it_prob_outdegree;
  it_prob_outdegree=probability_node_outdegree.find(index3);
  multimap<int,float> m_outdegree;
  m_outdegree=(*it_prob_outdegree).second;
  multimap<int,float>::iterator it_prob_out;
  it_prob_out=m_outdegree.find(index4);
  (*it_prob_out).second=p;
   (*it_prob_outdegree).second=m_outdegree;
 //Updation of layer probability

 int sum_layer=0;
 layerdegree[index]++;
 layerdegree[index3]++;
 for(int i=0;i<layerdegree.size();i++)
  {
     sum_layer+=layerdegree[i];
  }
 multimap<int,float>::iterator it_prob_layer;
 it_prob_layer=probability_layer.find(index);
 (*it_prob_layer).second=float(layerdegree[index])/float(sum_layer);
 it_prob_layer=probability_layer.find(index3);
 (*it_prob_layer).second=float(layerdegree[index3])/float(sum_layer);
 //Updation of size of network
 size++;
}
void display()
{
  //Displaying Inter Edge list
  cout<<"Intraedges----------------\n";
  for(multimap<int,multimap<int,int> >::iterator it=intraedge.begin();it!=intraedge.end();it++)
   {
     cout<<"Layer :"<<(*it).first<<"------------------------------\n";
     multimap<int,int> m;
     m=(*it).second;
     for(multimap<int,int>::iterator it1=m.begin();it1!=m.end();it1++)
      {
         cout<<(*it1).first<<" "<<(*it1).second<<endl;
      }

   }
  cout<<"Interedges----------------\n";
  for(multimap<int,multimap<int,multimap<int,int> > >::iterator it=interedge.begin();it!=interedge.end();it++)
   {
     cout<<"Layer :"<<(*it).first<<"------------------------------\n";
     multimap<int,multimap<int,int> > m;
     m=(*it).second;
     for(multimap<int,multimap<int,int> >::iterator it1=m.begin();it1!=m.end();it1++)
      {
         cout<<(*it1).first<<" ";
         multimap<int,int> m1;
         m1=(*it1).second;
         multimap<int,int>::iterator it2;
         it2=m1.begin();
         cout<<(*it2).first<<" "<<(*it2).second<<endl;
      }

   }
   //displaying probability indegree
  for(int i=0;i<n;i++)
   {
     multimap<int,multimap<int,float> >::iterator it;
     it=probability_node_indegree.find(i);
     multimap<int,float> m;
     m=(*it).second;
     cout<<"Layer "<<i<<" - ------------------------------------------------------------------------\n";
     for(multimap<int,float>::iterator it1=m.begin();it1!=m.end();it1++)
      {
        cout<<"Node "<<(*it1).first<<" probability "<<(*it1).second<<endl;
      }
   }
   //displaying indegree
 cout<<"INDEGREE-------------------------------------\n";
for(multimap<int,vector<int> >::iterator it=indegree.begin();it!=indegree.end();it++)
{
   vector<int> v =(*it).second;
   cout<<"Layer "<<(*it).first<<"----------"<<endl;
   for(int i=0;i<v.size();i++)
      cout<<i<<" "<<v[i]<<endl;
 }
 cout<<"Outdegree-----------------------------------\n";
for(multimap<int,vector<int> >::iterator it=outdegree.begin();it!=outdegree.end();it++)
{
   vector<int> v =(*it).second;
   cout<<"Layer "<<(*it).first<<"----------"<<endl;
   for(int i=0;i<v.size();i++)
      cout<<i<<" "<<v[i]<<endl;
 }
 //Displaying outdegree probability
 cout<<"Probability based on outdegree----------------------------\n";
 for(int i=0;i<n;i++)
   {
     multimap<int,multimap<int,float> >::iterator it;
     it=probability_node_outdegree.find(i);
     multimap<int,float> m;
     m=(*it).second;
     cout<<"Layer "<<i<<" - ------------------------------------------------------------------------\n";
     for(multimap<int,float>::iterator it1=m.begin();it1!=m.end();it1++)
      {
        cout<<"Node "<<(*it1).first<<" probability "<<(*it1).second<<endl;
      }
   }
  cout<<"Probability  of layer-----------------\n";
  for(multimap<int,float>::iterator it=probability_layer.begin();it!=probability_layer.end();it++)
   {
     cout<<"Layer "<<(*it).first<<"Probability "<<(*it).second<<endl;
   }

}
void random_attack()
{
  //choosing random layer for node to be removed
  int index=rand()%n;
  multimap<int,vector<int> >::iterator it;
  it=indegree.find(index);
  vector<int> v=(*it).second;
  int node=rand()%v.size();
 label: if(v[node]==0){
    node=(node+1)%v.size();
    if(v[node]==0)
      goto label;
    }
  cout<<"Layer is "<<index<<" and node is "<<node<<endl;
  //ramoving intradges with that node
  multimap<int,multimap<int,int> >::iterator itt;
  itt=intraedge.find(index);
  multimap<int,int> m=(*itt).second;
  for(multimap<int,int>::iterator it1=m.begin();it1!=m.end();it1++)
    {
       if((*it1).first==node||(*it1).second==node)
         m.erase(it1);
    }
   (*itt).second=m;
  //removing interedges with that node
  multimap<int,multimap<int,multimap<int,int> > >::iterator it_inter;
  it_inter=interedge.find(index);
  multimap<int,multimap<int,int> > m_inter;
  m_inter=(*it_inter).second;
  for(multimap<int,multimap<int,int> >::iterator it1=m_inter.begin();it1!=m_inter.end();it1++)
    {
      multimap<int,int> m2;
      m2=(*it1).second;
      multimap<int,int>::iterator it2=m2.begin();
      if((*it2).first==node)
        m_inter.erase(it1);
    }
  (*it_inter).second=m_inter;
 //updating indegree
 it=indegree.find(index);
 v=(*it).second;
 v[node]=0;
 (*it).second=v;
 //updating outdegree;
 it=outdegree.find(index);
 v=(*it).second;
 v[node]=0;
 (*it).second=v;
}
void calculate_clustering_coefficient()
{
  //Calculating clustering coefficient in which all the three nodes are in same layer
  for(int i=0;i<n;i++)
   {
       float sum=0;
       multimap<int,vector<int> >::iterator it;
       it=indegree.find(i);
       vector<int> v=(*it).second;
       for(int j=0;j<v.size();j++)
         {
             multimap<int,multimap<int,int> >::iterator it1;
             it1=intraedge.find(i);
             multimap<int,int> m=(*it1).second;
             vector<int> neighbour;
             for(multimap<int,int>::iterator it2=m.begin();it2!=m.end();it2++)
              {
                  if((*it2).first==j)
                    neighbour.push_back((*it2).first);
              }
             int count=0;
             //calculating no of pairs of neighbours having edges between them
             for(int k=0;k<neighbour.size();k++)
               {
                   for(int l=0;l<neighbour.size();l++)
                      {
                         if(k!=l)
                          {
                            for(multimap<int,int>::iterator it3=m.begin();it3!=m.end();it3++)
                              {
                                  if((*it3).first==k&&(*it3).second==l)
                                    count++;
                              }

                          }
                      }
               }
            float c;
            if(v[j]>=2)
             c=float(count/2)/(float(v[j])*(float(v[j])-1));
            else
                c=0;
            sum+=2*c;
         }
        cout<<"Layer  : "<<i<<endl;
        cout<<"Clustering Coefficient is "<<sum/float(v.size())<<endl;
    }
  //caculating clustering coefficient when two nodes are in one layer and one node in other layer
  float sum2=0;
  for(int i=0;i<n;i++)
   {
        multimap<int,vector<int> >::iterator it;
        it=indegree.find(i);
        multimap<int,vector<int> >::iterator itt=outdegree.find(i);
        vector<int> v1=(*itt).second;
        vector<int> v=(*it).second;
        for(int j=0;j<v.size();j++)
         {
            multimap<int,multimap<int,int> >::iterator it1=intraedge.find(i);
            multimap<int,int> m=(*it1).second;
            vector<int> neighbour_in;
            vector<int> neighbour_out;
            for(multimap<int,int>::iterator it2=m.begin();it2!=m.end();it2++)
             {
                if((*it2).first==j)
                  neighbour_in.push_back((*it2).second);

             }
            multimap<int,multimap<int,multimap<int,int> > >::iterator it3=interedge.find(i);
            multimap<int,multimap<int,int> > m1=(*it3).second;
            for(multimap<int,multimap<int,int> >::iterator it4=m1.begin();it4!=m1.end();it4++)
             {
                 multimap<int,int> m2=(*it4).second;
                 multimap<int,int>::iterator it5=m2.begin();
                 if((*it5).first==i)
                  neighbour_out.push_back((*it5).second);
             }
            int count=0;
            for(int k=0;k<neighbour_in.size();k++)
             {
               for(int l=0;l<neighbour_out.size();l++)
                 {
                     for(multimap<int,multimap<int,int> >::iterator it4=m1.begin();it4!=m1.end();it4++)
                       {
                          multimap<int,int> m2=(*it4).second;
                          multimap<int,int>::iterator it5=m2.begin();
                          if((*it5).first==k&&(*it5).second==l)
                            count++;
                      }
                 }
             }
            float c;
            if((v[j]+v1[j])>=2)
             c=float(count/2)/(float(v[j]+v1[j])*(float(v[j]+v1[j])-1));
            else
                c=0;
            sum2+=2*c;

         }
        cout<<"Layer  : "<<i<<endl;
        cout<<"Clustering Coefficient of second type is "<<sum2/float(v.size())<<endl;

   }
  //clustering coefficient of third kind in which each of three nodes are in different layer
   for(int i=0;i<n;i++)
   {
        multimap<int,vector<int> >::iterator it;
        it=indegree.find(i);
        multimap<int,vector<int> >::iterator itt=outdegree.find(i);
        vector<int> v1=(*itt).second;
        vector<int> v=(*it).second;
        for(int j=0;j<v.size();j++)
         {

            multimap<int,multimap<int,multimap<int,int> > >::iterator it3=interedge.find(i);
            multimap<int,multimap<int,int> > m1=(*it3).second;
            multimap<int,int> neighbour;
            for(multimap<int,multimap<int,int> >::iterator it4=m1.begin();it4!=m1.end();it4++)
             {
                 multimap<int,int> m2=(*it4).second;
                 multimap<int,int>::iterator it5=m2.begin();
                 if((*it5).first==i)
                   {
                     neighbour.insert(pair<int,int>((*it4).first,(*it5).second));
                   }
             }
            int count=0;
            for(multimap<int,int>::iterator it6=neighbour.begin();it6!=neighbour.end();it6++)
             {
                 for(multimap<int,int>::iterator it7=neighbour.begin();it7!=neighbour.end();it7++)
                   {
                     if((*it6).first!=(*it7).first)
                       {
                          int x=(*it6).second;
                          int y=(*it7).second;
                          multimap<int,multimap<int,multimap<int,int> > >::iterator it8=interedge.find((*it6).first);
                          multimap<int,multimap<int,int> > m3=(*it8).second;
                          for(multimap<int,multimap<int,int> >::iterator it9=m3.begin();it9!=m3.end();it9++)
                           {
                              if((*it9).first==(*it7).first)
                               {
                                  multimap<int,int> m4=(*it9).second;
                                  multimap<int,int>::iterator it10=m4.begin();
                                  if((*it10).first==x&&(*it10).second==y)
                                   count++;

                               }
                           }
                       }
                   }
             }
            float c;
            if((v[j]+v1[j])>=2)
             c=float(count/2)/(float(v[j]+v1[j])*(float(v[j]+v1[j])-1));
            else
                c=0;
            sum2+=2*c;

         }
        cout<<"Layer  : "<<i<<endl;
        cout<<"Clustering Coefficient of Third type is "<<sum2/float(v.size())<<endl;

   }

}
void degree_distribution()
{
   multimap<int,vector<int> > total_degree;
   for(int i=0;i<n;i++)
    {
       multimap<int,vector<int> >::iterator it1,it2;
       it1=indegree.find(i);
       it2=outdegree.find(i);
       vector<int> v1,v2;
       v1=(*it1).second;
       v2=(*it2).second;
       vector<int> v;
       for(int j=0;j<v1.size();j++)
        {
          int x=v1[j]+v2[j];
          v.push_back(x);
        }
        total_degree.insert(pair<int,vector<int> >(i,v));

    }
  fstream f1,f2,f3,f4,f5;
  f1.open("degree_distribution_layer1.txt",ios::out);
  f2.open("degree_distribution_layer2.txt",ios::out);
  f3.open("degree_distribution_layer3.txt",ios::out);
  f4.open("degree_distribution_layer4.txt",ios::out);
  f5.open("degree_distribution_layer5.txt",ios::out);
  for(int i=0;i<n;i++)
   {
      multimap<int,vector<int> >::iterator it;
      it=total_degree.find(i);
      vector<int> v;
      v=(*it).second;
      set<int> s;
      int count[10000]={0};
      for(int j=0;j<v.size();j++)
       {
         count[v[j]]++;
         s.insert(v[j]);
       }
      for(set<int>::iterator it1=s.begin();it1!=s.end();it1++)
       {
          if(i==0)
          f1<<*it1<<" "<<float(count[*it1])/float(v.size())<<endl;
          else if(i==1)
          f2<<*it1<<" "<<float(count[*it1])/float(v.size())<<endl;
          else if(i==2)
          f3<<*it1<<" "<<float(count[*it1])/float(v.size())<<endl;
          else if(i==3)
          f4<<*it1<<" "<<float(count[*it1])/float(v.size())<<endl;
          else if(i==4)
          f5<<*it1<<" "<<float(count[*it1])/float(v.size())<<endl;
        }

   }

}
int main()
{
   cout<<"Enter the no of layers\n";
   cin>>n;
   cout<<"Enter the no of seed nodes\n";
   int seed;
   cin>>seed;
   size=seed;
   //making intra layer edge
   for(int layer=0;layer<n;layer++)
   {
    multimap<int,int> m;
    for(int i=0;i<seed;i++)
    {
       for(int j=0;j<seed;j++)
          {
            if(i!=j)
            m.insert(pair<int,int>(i,j));
          }

     }
      intraedge.insert(pair<int,multimap<int,int> >(layer,m));
   }
//making inter layer edge
 srand(time(0));
  for(int i=0;i<n;i++)
    {
      int degoflayer=rand()%n;
      int layer1;
      multimap<int,multimap<int,int> > m1;
      for(int j=0;j<degoflayer;j++){
      cout<<"random no "<<rand()%100<<endl;
      layer1=i;
      cout<<"hello\n";
      int layer2=rand()%n;
      int node1=rand()%seed;
      int node2=rand()%seed;
        if(node1!=node2&&layer1!=layer2)
           {
             multimap<int,int> m;
             m.insert(pair<int,int>(node1,node2));
             m1.insert(pair<int,multimap<int,int> >(layer2,m));
           }
         }
       interedge.insert(pair<int,multimap<int,multimap<int,int> > >(i,m1));
    }
   multimap<int,multimap<int,multimap<int,int> > >::iterator it;

   for(it=interedge.begin();it!=interedge.end();it++)
    {
      cout<<(*it).first<<endl;
       multimap<int,multimap<int,int> > m;
       m=(*it).second;
       multimap<int,multimap<int,int> >::iterator it1;
       it1=m.begin();
       while(it1!=m.end())
        {
           multimap<int,int>::iterator it2;
           multimap<int,int> m1;
           m1=(*it1).second;
           it2=m1.begin();
           cout<<(*it).first<<" "<<(*it1).first<<" "<<(*it2).first<<" "<<(*it2).second<<endl;
           it1++;

        }

    }
  calculate_degree();
  int ch;
  int x;
  int no_of_node;
  float f=0;
  int remained;
  float fraction_removed=0;
  while(true){
  cout<<"Enter your choice\n";
  cout<<"1.Display()\n2.Random Attack\n3.Add new node\n4.Calculate the maximum size of connected components\n5.Clustering Coefficient\n6.Degree_distribution\n7.exit\n";
   cin>>ch;
  switch(ch){
   case 1 :
          display();
          break;
   case 2 :
          cout<<"Enter fraction of node to be removed\n";
           cin>>f;
            no_of_node=f*x;
            if(remained<no_of_node){
              cout<<"Sorry Attack is not possible\n";
              cout<<"Number of remaining node is "<<remained<<endl;
              cout<<"Number of attacked node is "<<no_of_node<<endl;
              goto label;
              }
           cout<<"No of node removed is "<<no_of_node<<endl;
           for(int i=0;i<no_of_node;i++){
                random_attack();
              }
              remained=remained-no_of_node;
              cout<<"Number of remaining node is "<<remained<<endl;
           label: break;
  case 3 :
          cout<<"Enter the size of your network\n";
          cin>>x;
          for(int i=0;i<x;i++){
          cout<<"count "<<i+1<<endl;
          AddNewNode();
          cout<<"-------------------------------------------------------\n";
          }
          remained=x+seed*n;
          break;
   case 4 :
          fraction_removed+=f;
          calculate_max_connected_component(fraction_removed);
          break;
   case 5 :
           calculate_clustering_coefficient();
           break;
   case 6 :
          degree_distribution();
          break;
   case 7 :
          exit(0);

   default :
           cout<<"Please enter right choice\n";
           break;

    }
  }
}
